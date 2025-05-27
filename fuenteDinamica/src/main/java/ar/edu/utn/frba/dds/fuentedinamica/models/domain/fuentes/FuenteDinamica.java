package ar.edu.utn.frba.dds.fuentedinamica.models.domain.fuentes;

import ar.edu.utn.frba.dds.fuentedinamica.models.domain.Hecho;
import ar.edu.utn.frba.dds.fuentedinamica.models.domain.Administrador;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;
import java.util.List;

@Getter
@Setter
public class FuenteDinamica implements Fuente {
  private Long id;
  private List<Hecho> hechos;
  private Administrador administrador;

  public void agregarHecho(Hecho hecho) {
    this.hechos.add(hecho);
  }

  @Override
  public List<Hecho> importarHechos() {
    return null; //TODO
  }


  @Override
  public String toString() {
    return "Dinámica";
  }
}
