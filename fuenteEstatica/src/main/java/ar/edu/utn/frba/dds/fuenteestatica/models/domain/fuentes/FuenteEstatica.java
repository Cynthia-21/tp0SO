package ar.edu.utn.frba.dds.fuenteestatica.models.domain.fuentes;

import ar.edu.utn.frba.dds.fuenteestatica.models.domain.Hecho;
import ar.edu.utn.frba.dds.fuenteestatica.models.domain.fuentes.estrategiasFuenteEstatica.EstrategiaImportacionEstatica;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;

import java.util.List;

@Getter@Setter

public class FuenteEstatica implements Fuente {

  private Long id;
  private String rutaFuente;
  private EstrategiaImportacionEstatica estartegiaDeImportacion;

  public FuenteEstatica(String ruta, EstrategiaImportacionEstatica estrategia){
    this.rutaFuente = ruta;
    this.estartegiaDeImportacion=estrategia;
  }
  @Override
  public List<Hecho> importarHechos() {
    return estartegiaDeImportacion.leerArchivo(this.rutaFuente);
  }

  @Override
  public String toString() {
    return "Estatica";
  }

}
