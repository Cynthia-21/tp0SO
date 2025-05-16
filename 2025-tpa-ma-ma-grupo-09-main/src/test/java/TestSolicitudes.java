import domain.Administrador;
import domain.Categoria;
import domain.Contribuyente;
import domain.Hecho;
import domain.Coleccion;
import domain.exceptions.HechoEliminadoException;
import domain.solicitudes.PosibleEstado;
import domain.solicitudes.MotivoSolicitud;
import domain.solicitudes.SolicitudEliminacion;
import domain.ubicacion.Coordenadas;
import domain.ubicacion.Ubicacion;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

import java.time.Duration;
import java.time.LocalDate;

public class TestSolicitudes {
  private Hecho hecho;
  private Contribuyente contribuyente;
  private Contribuyente contribuyente2;
  private Administrador administrador;
  private SolicitudEliminacion solicitudEliminacion1;
  private SolicitudEliminacion solicitudEliminacion2;
  private Coleccion coleccion;

  @BeforeEach
  public void init() {
    this.hecho = new Hecho("Brote de enfermedad contagiosa causa estragos en San Lorenzo, Santa Fe");
    this.hecho.setDescripcion("Grave brote de enfermedad contagiosa ocurrió en las inmediaciones de San Lorenzo, Santa Fe. El incidente dejó varios heridos y daños materiales. Se ha declarado estado de emergencia en la región para facilitar la asistencia.");
    this.hecho.agregarCategoria(new Categoria("Evento sanitario"));
    Ubicacion ubicacion = new Ubicacion(new Coordenadas(-32.786098, -60.741543));
    this.hecho.setUbicacion(ubicacion);
    LocalDate fecha = LocalDate.of(2005, 7,5);
    this.hecho.setFechaAcontecimiento(fecha);

    this.contribuyente = new Contribuyente("Lucas", "Bruno", 30);
    this.contribuyente2 = new Contribuyente("Paula", "Juan", 20);
    this.administrador = new Administrador("Cynthia", "Gonzalo");
  }

  @Test
  @DisplayName("Crear una solicitud de eliminación asociada a este hecho. Quedará en estado pendiente.")
  public void test1() {
    String motivoTexto = "La presente solicitud ha sido revisada conforme a los criterios establecidos por la normativa vigente y, tras un análisis detallado de su contenido, se ha determinado que no cumple con los requisitos formales y sustantivos necesarios para su aceptación. Se han identificado inconsistencias en los datos aportados, omisiones en la documentación obligatoria, y falta de justificación suficiente respecto al objetivo declarado en la misma. Asimismo, se advierte que el procedimiento requerido para este tipo de solicitudes no ha sido seguido en su totalidad, lo cual impide continuar con su tramitación en el estado actual. Por tales razones, y con el fin de mantener la transparencia y el cumplimiento de los procesos internos, se procede a la eliminación de la solicitud en cuestión, dejando constancia para futuras referencias.";
    MotivoSolicitud motivo = new MotivoSolicitud(motivoTexto);
    this.solicitudEliminacion1 = new SolicitudEliminacion(this.contribuyente, this.hecho, motivo);
    assertEquals(this.hecho, this.solicitudEliminacion1.getHechoAEliminar());
    assertEquals(PosibleEstado.PENDIENTE, this.solicitudEliminacion1.getEstadoSolicitudActual());
  }

  @Test
  @DisplayName("El motivo de esta solicitud deberá tener al menos 500 caracteres.")
  public void test2() {
    String motivoTexto = "La presente solicitud ha sido revisada conforme a los criterios establecidos por la normativa vigente y, tras un análisis detallado de su contenido, se ha determinado que no cumple con los requisitos formales y sustantivos necesarios para su aceptación. Se han identificado inconsistencias en los datos aportados, omisiones en la documentación obligatoria, y falta de justificación suficiente respecto al objetivo declarado en la misma. Asimismo, se advierte que el procedimiento requerido para este tipo de solicitudes no ha sido seguido en su totalidad, lo cual impide continuar con su tramitación en el estado actual. Por tales razones, y con el fin de mantener la transparencia y el cumplimiento de los procesos internos, se procede a la eliminación de la solicitud en cuestión, dejando constancia para futuras referencias. Ese es mi motivo";
    MotivoSolicitud motivo = new MotivoSolicitud(motivoTexto);
    this.solicitudEliminacion1 = new SolicitudEliminacion(this.contribuyente, this.hecho, motivo);
    assertThrows(IllegalArgumentException.class, () -> {new SolicitudEliminacion(this.contribuyente, this.hecho, new MotivoSolicitud("Jorge"));});
  }

  @Test
  @DisplayName("Rechazar esta solicitud un día después de su creación. Dado que fue rechazada, el hecho puede ser agregado a cualquier colección.")
  public void test3() {
    String motivoTexto = "La presente solicitud ha sido revisada conforme a los criterios establecidos por la normativa vigente y, tras un análisis detallado de su contenido, se ha determinado que no cumple con los requisitos formales y sustantivos necesarios para su aceptación. Se han identificado inconsistencias en los datos aportados, omisiones en la documentación obligatoria, y falta de justificación suficiente respecto al objetivo declarado en la misma. Asimismo, se advierte que el procedimiento requerido para este tipo de solicitudes no ha sido seguido en su totalidad, lo cual impide continuar con su tramitación en el estado actual. Por tales razones, y con el fin de mantener la transparencia y el cumplimiento de los procesos internos, se procede a la eliminación de la solicitud en cuestión, dejando constancia para futuras referencias. Ese es mi motivo";
    MotivoSolicitud motivo = new MotivoSolicitud(motivoTexto);
    this.solicitudEliminacion1 = new SolicitudEliminacion(this.contribuyente, this.hecho, motivo);
    solicitudEliminacion1.rechazarSolicitud(this.solicitudEliminacion1.getFechaSolicitud().plusDays(1), this.administrador);
    assertEquals(PosibleEstado.RECHAZADA, this.solicitudEliminacion1.getEstadoSolicitudActual());
    Duration duracion = Duration.between(this.solicitudEliminacion1.getFechaSolicitud(), this.solicitudEliminacion1.getFechaUltimaRevision());
    assertTrue(duracion.toHours() >= 24);

    this.coleccion = new Coleccion("Colección prueba");
    assertDoesNotThrow(() -> coleccion.agregarHecho(hecho));
  }

  @Test
  @DisplayName("Generar otra solicitud para el mismo hecho.")
  public void test4() {
    String motivoTexto = "La solicitud fue eliminada debido a que no cumple con los requisitos establecidos en el reglamento vigente. Se detectaron inconsistencias en los datos provistos, falta de documentación obligatoria y ausencia de justificación clara sobre el objetivo planteado. Además, fue presentada fuera del plazo correspondiente. Por estos motivos, no es posible continuar con el trámite y se procede a su eliminación del sistema. Esta decisión se toma en cumplimiento con las políticas internas de control, asegurando la transparencia en el proceso y garantizando que únicamente las solicitudes que cumplan con todos los requisitos puedan ser procesadas. Se deja constancia para futuras referencias y auditorías.";
    MotivoSolicitud motivo = new MotivoSolicitud(motivoTexto);
    this.solicitudEliminacion2 = new SolicitudEliminacion(this.contribuyente2, this.hecho, motivo);
    assertEquals(this.hecho, this.solicitudEliminacion2.getHechoAEliminar());
    assertEquals(PosibleEstado.PENDIENTE, this.solicitudEliminacion2.getEstadoSolicitudActual());
  }

  @Test
  @DisplayName("Ahora esta solicitud es aceptada a las 2 horas. Esta vez el hecho no debería poder agregarse a una colección, puesto que este fue eliminado.")
  public void test5() {
    String motivoTexto = "La solicitud fue eliminada debido a que no cumple con los requisitos establecidos en el reglamento vigente. Se detectaron inconsistencias en los datos provistos, falta de documentación obligatoria y ausencia de justificación clara sobre el objetivo planteado. Además, fue presentada fuera del plazo correspondiente. Por estos motivos, no es posible continuar con el trámite y se procede a su eliminación del sistema. Esta decisión se toma en cumplimiento con las políticas internas de control, asegurando la transparencia en el proceso y garantizando que únicamente las solicitudes que cumplan con todos los requisitos puedan ser procesadas. Se deja constancia para futuras referencias y auditorías.";
    MotivoSolicitud motivo = new MotivoSolicitud(motivoTexto);
    this.solicitudEliminacion2 = new SolicitudEliminacion(this.contribuyente2, this.hecho, motivo);
    this.solicitudEliminacion2.aceptarSolicitud(this.solicitudEliminacion2.getFechaSolicitud().plusHours(2), this.administrador);
    assertEquals(PosibleEstado.CONFIRMADA, this.solicitudEliminacion2.getEstadoSolicitudActual());
    Duration duracion = Duration.between(this.solicitudEliminacion2.getFechaSolicitud(), this.solicitudEliminacion2.getFechaUltimaRevision());
    assertEquals(2, duracion.toHours());

    this.coleccion = new Coleccion("Colección prueba");
    assertThrows(HechoEliminadoException.class, () -> coleccion.agregarHecho(hecho));
  }

  @Test
  @DisplayName("Despues de que el administrador acepte una solicitud de eliminación, se solicita una nueva revisión donde es rechazada")
  public void test6() {
    String motivoTexto = "La solicitud fue eliminada debido a que no cumple con los requisitos establecidos en el reglamento vigente. Se detectaron inconsistencias en los datos provistos, falta de documentación obligatoria y ausencia de justificación clara sobre el objetivo planteado. Además, fue presentada fuera del plazo correspondiente. Por estos motivos, no es posible continuar con el trámite y se procede a su eliminación del sistema. Esta decisión se toma en cumplimiento con las políticas internas de control, asegurando la transparencia en el proceso y garantizando que únicamente las solicitudes que cumplan con todos los requisitos puedan ser procesadas. Se deja constancia para futuras referencias y auditorías.";
    MotivoSolicitud motivo = new MotivoSolicitud(motivoTexto);
    this.solicitudEliminacion2 = new SolicitudEliminacion(this.contribuyente2, this.hecho, motivo);

    this.solicitudEliminacion2.aceptarSolicitud(this.solicitudEliminacion2.getFechaSolicitud().plusHours(2), this.administrador);
    assertEquals(PosibleEstado.CONFIRMADA, this.solicitudEliminacion2.getEstadoSolicitudActual());
    assertEquals(this.administrador, this.solicitudEliminacion2.getUltimoEstadoSolicitud().getAdministrador());
    assertTrue(this.hecho.getEstaEliminado());

    this.solicitudEliminacion2.rechazarSolicitud(this.solicitudEliminacion2.getFechaUltimaRevision().plusHours(2), this.administrador);
    assertEquals(PosibleEstado.RECHAZADA, this.solicitudEliminacion2.getEstadoSolicitudActual());
    assertFalse(this.hecho.getEstaEliminado());
  }
}
