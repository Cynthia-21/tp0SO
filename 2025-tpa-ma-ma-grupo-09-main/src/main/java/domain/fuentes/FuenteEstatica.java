package domain.fuentes;

import domain.Hecho;
import domain.fuentes.estrategiasFuenteEstatica.EstrategiaImportacionEstatica;
import java.util.List;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;

@Getter@Setter
@AllArgsConstructor
public class FuenteEstatica implements Fuente {

  private String rutaFuente;
  private EstrategiaImportacionEstatica estartegiaDeImportacion;

  public FuenteEstatica() {}
  @Override
  public List<Hecho> importarHechos() {
    return estartegiaDeImportacion.leerArchivo(this.rutaFuente);
  }

  @Override
  public String toString() {
    return "Estatica";
  }

}
